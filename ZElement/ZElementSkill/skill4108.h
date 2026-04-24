#ifndef __CPPGEN_GNET_SKILL4108
#define __CPPGEN_GNET_SKILL4108
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4108:public Skill
    {
      public:
        enum
        { SKILL_ID = 4108 };
          Skill4108 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4108Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar11 (skill->GetPlayer ()->GetOccupation ());
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetMaxhp ());
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetMaxmp ());
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetHp ());
                skill->GetPlayer ()->SetVar5 (skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetVar6 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetVar7 (skill->GetPlayer ()->GetRes2 ());
                skill->GetPlayer ()->SetVar8 (skill->GetPlayer ()->GetRes3 ());
                skill->GetPlayer ()->SetVar9 (skill->GetPlayer ()->GetRes4 ());
                skill->GetPlayer ()->SetVar10 (skill->GetPlayer ()->GetRes5 ());
                skill->GetPlayer ()->SetVar16 (zrand (100) < (skill->GetLevel () + 1) * (skill->GetLevel () + 1) ? 1 : 2);
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes2 ());
                skill->SetRatio (1 + 0.07 * skill->GetLevel () + 0.01 * INT (skill->GetPlayer ()->GetLevel () / (9 - skill->GetLevel ())));
                skill->SetPlus (225 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4108Stub ():SkillStub (4108)
        {
            occupation = 130;
            name = L"Õæ¡¤±ÌÌìÓî»ª";
            nativename = "Õæ¡¤±ÌÌìÓî»ª";
            icon = "·ÉÉı±¦¿âÖ®»ê.dds";
            maxlevel = 5;
            maxlearn = 5;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            talent[0] = 687;
            talent[1] = 1582;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_·¨±¦_±¦¿âÖ®»ê";
            action[1] = "ÇàÔÆÃÅ_·¨±¦_±¦¿âÖ®»ê";
            action[2] = "¹íÍõ×Ú_·¨±¦_±¦¿âÖ®»ê";
            action[3] = "ºÏ»¶ÅÉ_·¨±¦_±¦¿âÖ®»ê";
            action[4] = "ÌìÒôËÂ_·¨±¦_±¦¿âÖ®»ê";
            action[5] = "¹íµÀ_·¨±¦_±¦¿âÖ®»ê";
            action[6] = "·ÙÏã¹È_·¨±¦_±¦¿âÖ®»ê";
            action[7] = "ÊŞÉñ_·¨±¦_±¦¿âÖ®»ê";
            action[8] = "ºüÑı_·¨±¦_±¦¿âÖ®»ê";
            action[9] = "ĞùÔ¯_·¨±¦_±¦¿âÖ®»ê";
            action[10] = "»³¹â_·¨±¦_±¦¿âÖ®»ê";
            action[11] = "Ì«ê»_·¨±¦_±¦¿âÖ®»ê";
            action[12] = "Ìì»ª_·¨±¦_±¦¿âÖ®»ê";
            action[13] = "ÁéÙí_·¨±¦_±¦¿âÖ®»ê";
            action[14] = "Ó¢ÕĞ_·¨±¦_±¦¿âÖ®»ê";
            action[15] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[17] = "¹íÍõ×Ú_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[19] = "ÌìÒôËÂ_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[20] = "¹íµÀ_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[21] = "·ÙÏã¹È_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[22] = "ÊŞÉñ_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[23] = "ºüÑı_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[24] = "ĞùÔ¯_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[25] = "»³¹â_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[26] = "Ì«ê»_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[27] = "Ìì»ª_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[28] = "ÁéÙí_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[29] = "Ó¢ÕĞ_Æï³Ë_·¨±¦_±¦¿âÖ®»ê";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill4108Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 10);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (445 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 110000 - 10000 * skill->GetLevel ();
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05 + 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (0.5 * skill->GetPlayer ()->GetRange () - 0.3 * INT (skill->GetPlayer ()->GetRange () / 3.9) +
                            1.8 * INT (skill->GetPlayer ()->GetRange () / 8.9) + 2.8 * INT (skill->GetPlayer ()->GetRange () / 13) + 0.05);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               7 * skill->GetLevel (),
                               225 * skill->GetLevel (),
                               9 - skill->GetLevel (),
                               445 - 35 * skill->GetLevel () - skill->GetLevel () * skill->GetLevel (), 110 - 10 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetTalismaneffects (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTalismaneffectsbless (1);
            return true;
        }
#endif
    };
}
#endif
