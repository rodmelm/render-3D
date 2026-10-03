#include "cilinder.hpp"
#include "intersection_info.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector.hpp"
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <optional>

namespace render {
  namespace {  // Namespace anónimo para las funciones internas

    // ESTRUCTURAS PARA DATOS TEMPORALES ----

    // Estructura para agrupar datos de intersección
    struct InterseccionDatos {
      double t;
      vector punto;
      vector normal;
      bool frente;
      bool encontrada;
    };

    // Estructura para coeficientes de ecuación cuadrática
    struct CoeficientesCurva {
      double a, b, c;
    };

    struct PlanoBase {
      vector punto, normal;
    };

    // Calcula coeficientes para superficie curva
    CoeficientesCurva calcular_coeficientes_curva(vector const & rc, vector const & dir,
                                                  cilinder const & cilindro) {
      vector const eje_unitario = cilindro.geteje().normalize();
      // Vectores perpendiculares al eje
      vector const dir_perp = dir - eje_unitario * eje_unitario.producto(dir);
      vector const rc_perp  = rc - eje_unitario * eje_unitario.producto(rc);

      // Coeficientes de la fórmula cuadrática
      double const a = dir_perp.producto(dir_perp);
      double const b = 2 * rc_perp.producto(dir_perp);
      double const c = rc_perp.producto(rc_perp) - cilindro.getradio() * cilindro.getradio();

      return {a, b, c};
    }

    // Analizamos las soluciones de la ecuación cuadrática
    InterseccionDatos procesar_solucion_curva(ray const & rayo, cilinder const & cilindro,
                                              double const t, double const media_altura) {
      vector const eje_unitario = cilindro.geteje().normalize();
      vector const punto        = rayo.punto_at(t);
      double const proyeccion   = (punto - cilindro.getcentro()).producto(eje_unitario);

      // El punto solo es válido si no está por fuera de la altura del cilindro
      if (std::abs(proyeccion) <= media_altura) [[likely]] {
        double const t_curva     = t;
        vector const punto_curva = punto;
        vector normal_curva = ((punto_curva - cilindro.getcentro()) - eje_unitario * proyeccion);
        bool const frente_curva = (rayo.direccion.producto(normal_curva) < 0);
        // Si el sentido del vector normal es hacia adentro, cambia el signo
        if (!frente_curva) [[unlikely]] {
          normal_curva = normal_curva * -1;
        }
        return InterseccionDatos{t_curva, punto_curva, normal_curva, frente_curva, true};
      }
      return InterseccionDatos{
        std::numeric_limits<double>::max(), {0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0},
         true, false
      };
    }

    // Calculamos si hay intersección con una base del cilindro
    InterseccionDatos calcular_interseccion_base(ray const & rayo, cilinder const & cilindro,
                                                 InterseccionDatos const & interseccion_base,
                                                 PlanoBase const & plano) {
      vector const rp_sup          = plano.punto - rayo.origen;
      double const numerador_sup   = rp_sup.producto(plano.normal);
      double const denominador_sup = rayo.direccion.producto(plano.normal);

      // No se considera que haya intersección si el denominador es muy pequeño (el punto está muy
      // lejos)
      if (std::abs(denominador_sup) >= 1e-8) [[likely]] {
        double const t_sup = numerador_sup / denominador_sup;

        if (t_sup > 0.001 and t_sup < interseccion_base.t) {
          vector const punto     = rayo.punto_at(t_sup);
          vector const distancia = punto - plano.punto;

          // Comprobamos que el punto está dentro de la base
          if (distancia.magnitude() <= cilindro.getradio()) [[likely]] {
            double const t_base     = t_sup;
            vector const punto_base = punto;
            vector normal_base      = plano.normal;
            bool const frente_base  = (rayo.direccion.producto(normal_base) < 0);
            if (!frente_base) [[unlikely]] {
              normal_base = normal_base * -1;
            }
            return InterseccionDatos{t_base, punto_base, normal_base, frente_base, true};
          }
        }
        return interseccion_base;
      }
      return interseccion_base;
    }

    // Calculamos si hay intersección con la superficie curva
    InterseccionDatos interseccion_con_curva(cilinder const & cilindro, ray const & rayo,
                                             double media_altura) {
      vector const rc  = rayo.origen - cilindro.getcentro();
      vector const dir = rayo.direccion;

      CoeficientesCurva const coeficientes = calcular_coeficientes_curva(rc, dir, cilindro);
      double raiz = coeficientes.b * coeficientes.b - 4 * coeficientes.a * coeficientes.c;

      InterseccionDatos interseccion_curva{
        std::numeric_limits<double>::max(), {0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0},
         true, false
      };

      // Si la raiz es no negativa existen soluciones (intersecciones)
      if (raiz >= 0) [[likely]] {
        raiz            = std::sqrt(raiz);
        double const t1 = (-coeficientes.b - raiz) / (2 * coeficientes.a);
        double const t2 = (-coeficientes.b + raiz) / (2 * coeficientes.a);

        // Probamos t1
        if (t1 > 0.001) [[likely]] {
          interseccion_curva = procesar_solucion_curva(rayo, cilindro, t1, media_altura);
        }

        // Probamos t2
        if (t2 < interseccion_curva.t and t2 > 0.001) [[likely]] {
          interseccion_curva = procesar_solucion_curva(rayo, cilindro, t2, media_altura);
        }
      }
      return interseccion_curva;
    }

    // Calculamos la intersección con las bases más cercana
    InterseccionDatos interseccion_con_bases(cilinder const & cilindro, ray const & rayo,
                                             double media_altura) {
      InterseccionDatos interseccion_base{
        std::numeric_limits<double>::max(), {0.0, 0.0, 0.0},
         {0.0, 0.0, 0.0},
         true, false
      };

      vector const eje_unitario = cilindro.geteje().normalize();

      // INTERSECCIÓN CON LA BASE SUPERIOR
      vector const punto_sup  = cilindro.getcentro() + eje_unitario * (media_altura);
      vector const normal_sup = eje_unitario;
      PlanoBase const plano_sup{punto_sup, normal_sup};
      interseccion_base = calcular_interseccion_base(rayo, cilindro, interseccion_base, plano_sup);

      // INTERSECCIÓN CON LA BASE INFERIOR
      vector const punto_inf  = cilindro.getcentro() - eje_unitario * (media_altura);
      vector const normal_inf = eje_unitario * -1.0;
      PlanoBase const plano_inf{punto_inf, normal_inf};
      interseccion_base = calcular_interseccion_base(rayo, cilindro, interseccion_base, plano_inf);

      return interseccion_base;
    }

    // Calculamos cuál de las intersecciones que hemos encontrado es la más cercana
    std::optional<intersection_info> interseccion_mas_cercana(
        InterseccionDatos const & int_curva, InterseccionDatos const & int_base,
        std::shared_ptr<material> const & mat_val) {
      if (int_curva.encontrada and int_base.encontrada) [[unlikely]] {
        if (int_curva.t < int_base.t) [[likely]] {
          return intersection_info{int_curva.t, int_curva.punto, int_curva.normal, mat_val,
                                   int_curva.frente};
        }
        return intersection_info{int_base.t, int_base.punto, int_base.normal, mat_val,
                                 int_base.frente};
      }
      if (int_curva.encontrada) [[likely]] {
        return intersection_info{int_curva.t, int_curva.punto, int_curva.normal, mat_val,
                                 int_curva.frente};
      }
      if (int_base.encontrada) [[unlikely]] {
        return intersection_info{int_base.t, int_base.punto, int_base.normal, mat_val,
                                 int_base.frente};
      }
      return std::nullopt;
    }

  }  // namespace

  // FUNCIÓN PRINCIPAL
  // -----------------------------------------------------------------------------------------------
  std::optional<intersection_info> cilinder::intersect(ray const & rayo) const {
    double const media_altura = altura / 2.0;
    // Hallamos la intersección con la superficie curva
    InterseccionDatos const interseccion_curva = interseccion_con_curva(*this, rayo, media_altura);

    // Hallamos la intersección con las bases más cercana
    InterseccionDatos const interseccion_bases = interseccion_con_bases(*this, rayo, media_altura);

    // Seleccionamos la intersección más cercana
    return interseccion_mas_cercana(interseccion_curva, interseccion_bases, material_);
  }

}  // namespace render
